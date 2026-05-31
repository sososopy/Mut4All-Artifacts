//source file
#include "../include/Template_Function_Pointer_Ambiguity_47.h"

// ========================================================================================================
#define MUT47_OUTPUT 1

void MutatorFrontendAction_47::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->getTemplatedDecl()->getReturnType()->isUndeducedType()) {
            auto funcName = FD->getNameAsString();
            std::string mutation = "decltype(auto) ptr = &" + funcName + "; /*mut47*/";
            SourceLocation insertLoc = FD->getTemplatedDecl()->getBody()->getBeginLoc();
            Rewrite.InsertText(insertLoc, mutation, true, true);
        }
    }
}

void MutatorFrontendAction_47::MutatorASTConsumer_47::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(
        hasDescendant(functionDecl(returns(autoType())))
    ).bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}