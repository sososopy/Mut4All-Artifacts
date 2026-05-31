//source file
#include "../include/insert_virtual_base_class_258.h"

// ========================================================================================================
#define MUT258_OUTPUT 1

void MutatorFrontendAction_258::Callback::run(const MatchFinder::MatchResult &Result) {
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
        
        auto base_class = base_classes[getrandom::getRandomIndex(base_classes.size())];
        std::string base_class_name = base_class->getNameAsString();
        
        if (base_class_name.empty())
            return;

        std::string virtual_base_decl = " : virtual public " + base_class_name;
        std::string insertion = "/*mut258*/" + virtual_base_decl;
        
        Rewrite.InsertTextAfterToken(DL->getBeginLoc(), insertion);
    }
}

void MutatorFrontendAction_258::MutatorASTConsumer_258::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher baseMatcher = cxxRecordDecl().bind("BaseClass");
    DeclarationMatcher derivedMatcher = cxxRecordDecl().bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}