//source file
#include "../include/insert_virtual_base_class_404.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang;
using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT404_OUTPUT 1

void MutatorFrontendAction_404::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const CXXRecordDecl *BaseClass = Result.Nodes.getNodeAs<CXXRecordDecl>("BaseClass")) {
        if (!BaseClass || !Result.Context->getSourceManager().isWrittenInMainFile(BaseClass->getBeginLoc()))
            return;
        if (!BaseClass->isCompleteDefinition() || BaseClass->isLambda())
            return;

        std::string baseClassName = BaseClass->getNameAsString();
        if (baseClassName.empty() || visitedClasses.count(baseClassName))
            return;

        visitedClasses.insert(baseClassName);
    }
    else if (const CXXRecordDecl *DerivedClass = Result.Nodes.getNodeAs<CXXRecordDecl>("DerivedClass")) {
        if (!DerivedClass || !Result.Context->getSourceManager().isWrittenInMainFile(DerivedClass->getBeginLoc()))
            return;
        if (!DerivedClass->isCompleteDefinition() || DerivedClass->isLambda())
            return;

        std::string derivedClassName = DerivedClass->getNameAsString();
        if (derivedClassName.empty())
            return;

        std::string virtualBaseInsertion = " : virtual public " + *visitedClasses.begin();
        std::string insertionText = "class " + derivedClassName + virtualBaseInsertion + " {};\n";
        SourceLocation insertionLoc = DerivedClass->getEndLoc();

        Rewrite.InsertTextAfterToken(insertionLoc, ";\n/*mut404*/" + insertionText);
    }
}

void MutatorFrontendAction_404::MutatorASTConsumer_404::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher baseMatcher = cxxRecordDecl(unless(isDerivedFrom(cxxRecordDecl()))).bind("BaseClass");
    DeclarationMatcher derivedMatcher = cxxRecordDecl(isDerivedFrom(cxxRecordDecl())).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}