/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <test/calc_unoapi_test.hxx>
#include <test/sheet/xcellrangesquery.hxx>
#include <test/sheet/cellproperties.hxx>
#include <test/util/xreplaceable.hxx>
#include <test/util/xsearchable.hxx>
#include <test/sheet/xcellrangedata.hxx>

#include <com/sun/star/sheet/XSpreadsheetDocument.hpp>
#include <com/sun/star/sheet/XSpreadsheet.hpp>
#include <com/sun/star/table/XCellRange.hpp>
#include <com/sun/star/util/XSortable.hpp>
#include <com/sun/star/util/SortField.hpp>

using namespace css;
using namespace css::uno;

namespace sc_apitest {

#define NUMBER_OF_TESTS 15

class ScCellRangeObj : public CalcUnoApiTest, public apitest::XCellRangesQuery, public apitest::CellProperties,
                        public apitest::XSearchable, public apitest::XReplaceable, public apitest::XCellRangeData
{
public:
    ScCellRangeObj();

    virtual void setUp() override;
    virtual void tearDown() override;
    virtual uno::Reference< uno::XInterface > init() override;
    virtual uno::Reference< uno::XInterface > getXCellRangeData() override;
    void testSortOOB();


    CPPUNIT_TEST_SUITE(ScCellRangeObj);
    CPPUNIT_TEST(testQueryColumnDifference);
    CPPUNIT_TEST(testQueryContentDifference);
    CPPUNIT_TEST(testQueryEmptyCells);
    //CPPUNIT_TEST(testQueryFormulaCells);
    CPPUNIT_TEST(testQueryIntersection);
    CPPUNIT_TEST(testQueryRowDifference);
    CPPUNIT_TEST(testQueryVisibleCells);
    CPPUNIT_TEST(testVertJustify);
    CPPUNIT_TEST(testRotateReference);
    CPPUNIT_TEST(testFindAll);
    CPPUNIT_TEST(testFindFirst);
    CPPUNIT_TEST(testReplaceAll);
    CPPUNIT_TEST(testCreateReplaceDescriptor);
    CPPUNIT_TEST(testGetDataArray);
    CPPUNIT_TEST(testSetDataArray);
    CPPUNIT_TEST(testSortOOB);
    CPPUNIT_TEST_SUITE_END();

private:
    static sal_Int32 nTest;
    static uno::Reference< lang::XComponent > mxComponent;
};

sal_Int32 ScCellRangeObj::nTest = 0;
uno::Reference< lang::XComponent > ScCellRangeObj::mxComponent;

ScCellRangeObj::ScCellRangeObj():
        CalcUnoApiTest("/sc/qa/extras/testdocuments"),
        apitest::XSearchable("15", 1),
        apitest::XReplaceable("15", "35")
{
}

uno::Reference< uno::XInterface > ScCellRangeObj::init()
{
    OUString aFileURL;
    const OUString aFileBase("xcellrangesquery.ods");
    createFileURL(aFileBase, aFileURL);
    std::cout << OUStringToOString(aFileURL, RTL_TEXTENCODING_UTF8).getStr() << std::endl;
    if( !mxComponent.is())
        mxComponent = loadFromDesktop(aFileURL, "com.sun.star.sheet.SpreadsheetDocument");
    uno::Reference< sheet::XSpreadsheetDocument> xDoc (mxComponent, UNO_QUERY_THROW);
    uno::Reference< container::XIndexAccess > xIndex (xDoc->getSheets(), UNO_QUERY_THROW);
    uno::Reference< sheet::XSpreadsheet > xSheet( xIndex->getByIndex(0), UNO_QUERY_THROW);

    CPPUNIT_ASSERT_MESSAGE("Could not create interface of type XSpreadsheet", xSheet.is());

    uno::Reference<table::XCellRange> xReturn(xSheet->getCellRangeByPosition(0,0,3,4), UNO_QUERY_THROW);

    CPPUNIT_ASSERT_MESSAGE("Could not create object of type XCellRangesQuery", xReturn.is());
    return xReturn;
}

uno::Reference< uno::XInterface > ScCellRangeObj::getXCellRangeData()
{
    OUString aFileURL;
    const OUString aFileBase("xcellrangesquery.ods");
    createFileURL(aFileBase, aFileURL);
    std::cout << OUStringToOString(aFileURL, RTL_TEXTENCODING_UTF8).getStr() << std::endl;
    if( !mxComponent.is())
        mxComponent = loadFromDesktop(aFileURL, "com.sun.star.sheet.SpreadsheetDocument");
    uno::Reference< sheet::XSpreadsheetDocument> xDoc (mxComponent, UNO_QUERY_THROW);
    uno::Reference< container::XIndexAccess > xIndex (xDoc->getSheets(), UNO_QUERY_THROW);
    uno::Reference< sheet::XSpreadsheet > xSheet( xIndex->getByIndex(1), UNO_QUERY_THROW);

    CPPUNIT_ASSERT_MESSAGE("Could not create interface of type XSpreadsheet", xSheet.is());

    uno::Reference<table::XCellRange> xReturn(xSheet->getCellRangeByPosition(0,0,3,3), UNO_QUERY_THROW);

    CPPUNIT_ASSERT_MESSAGE("Could not create object of type XCellRangesQuery", xReturn.is());
    return xReturn;
}

void ScCellRangeObj::testSortOOB()
{
    uno::Reference<util::XSortable> xSortable(init(),UNO_QUERY_THROW);
    uno::Sequence<beans::PropertyValue> aEmptyDescriptor;
    xSortable->sort(aEmptyDescriptor);

    uno::Sequence<beans::PropertyValue> aProps(1);
    uno::Sequence<util::SortField> aSort(1);

    aSort[0].Field = 0xffffff;
    aSort[0].SortAscending = true;

    aProps[0].Name = "SortFields";
    aProps[0].Value <<= aSort;

    xSortable->sort(aProps);
}

void ScCellRangeObj::setUp()
{
    nTest++;
    CPPUNIT_ASSERT(nTest <= NUMBER_OF_TESTS);
    CalcUnoApiTest::setUp();
}

void ScCellRangeObj::tearDown()
{
    if (nTest == NUMBER_OF_TESTS)
    {
        closeDocument(mxComponent);
        mxComponent.clear();
    }

    CalcUnoApiTest::tearDown();
}

CPPUNIT_TEST_SUITE_REGISTRATION(ScCellRangeObj);

}

CPPUNIT_PLUGIN_IMPLEMENT();

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
