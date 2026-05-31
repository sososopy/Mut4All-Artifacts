//source file
#include "../include/insert_virtual_base_class_461.h"

// ========================================================================================================
#define MUT461_OUTPUT 1

void MutatorFrontendAction_461::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BaseClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClass")) {
        if (!BaseClass || !Result.Context->getSourceManager().isWrittenInMainFile(BaseClass->getLocation()))
            return;
        if (!BaseClass->isCompleteDefinition() || BaseClass->isLambda())
            return;

        std::string baseClassName = BaseClass->getNameAsString();
        if (baseClassName.empty() || visitedClasses.count(baseClassName))
            return;

        visitedClasses.insert(baseClassName);
        std::string virtualBaseClassDecl = "class VirtualBase_" + baseClassName + " : virtual public " + baseClassName + " {};";
        Rewrite.InsertTextAfter(BaseClass->getEndLoc(), "\n/*mut461*/" + virtualBaseClassDecl + "\n");
    }
}

void MutatorFrontendAction_461::MutatorASTConsumer_461::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher baseClassMatcher = cxxRecordDecl(isDefinition()).bind("BaseClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseClassMatcher, &callback);
    matchFinder.matchAST(Context);
}