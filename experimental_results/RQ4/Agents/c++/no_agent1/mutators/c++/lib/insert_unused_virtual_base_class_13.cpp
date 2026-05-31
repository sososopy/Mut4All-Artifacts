//source file
#include "../include/insert_unused_virtual_base_class_13.h"

// ========================================================================================================
#define MUT13_OUTPUT 1

void MutatorFrontendAction_13::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClass")) {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
            return;
        if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
            return;
        if (!CL->isCompleteDefinition())
            return;
        
        base_classes.push_back(CL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(DL->getLocation()))
            return;
        if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
            return;
        if (!DL->isCompleteDefinition())
            return;

        if (base_classes.empty())
            return;

        const clang::CXXRecordDecl *baseClass = base_classes.back();
        std::string baseClassName = baseClass->getNameAsString();
        std::string derivedClassName = DL->getNameAsString();

        std::string virtualBaseInsertion = "class " + derivedClassName + " : virtual public " + baseClassName + " {};";

        Rewrite.InsertTextAfterToken(DL->getEndLoc(), "\n/*mut13*/" + virtualBaseInsertion);
    }
}
  
void MutatorFrontendAction_13::MutatorASTConsumer_13::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher baseMatcher = cxxRecordDecl().bind("BaseClass");
    DeclarationMatcher derivedMatcher = cxxRecordDecl().bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}