//source file
#include "../include/modify_function_declaration_return_type_569.h"

// ========================================================================================================
#define MUT569_OUTPUT 1

void MutatorFrontendAction_569::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->hasBody() && !FD->isMain()) {
            auto returnType = FD->getReturnType();
            if (!returnType->isVoidType()) {
                SourceLocation startLoc = FD->getReturnTypeSourceRange().getBegin();
                Rewrite.ReplaceText(startLoc, returnType.getAsString().length(), "void");
            }
        }
    }
}

void MutatorFrontendAction_569::MutatorASTConsumer_569::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(unless(isMain())).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}