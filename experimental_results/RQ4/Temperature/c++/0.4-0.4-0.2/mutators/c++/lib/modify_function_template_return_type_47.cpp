//source file
#include "../include/modify_function_template_return_type_47.h"

// ========================================================================================================
#define MUT47_OUTPUT 1

void MutatorFrontendAction_47::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        const FunctionDecl *FuncDecl = FD->getTemplatedDecl();
        if (FuncDecl->getReturnType()->isUndeducedType()) {
            std::string returnType = "int";  // Change to a known type
            SourceLocation StartLoc = FuncDecl->getReturnTypeSourceRange().getBegin();
            SourceLocation EndLoc = FuncDecl->getReturnTypeSourceRange().getEnd();
            Rewrite.ReplaceText(SourceRange(StartLoc, EndLoc), returnType);
        }
    } else if (auto *DRE = Result.Nodes.getNodeAs<clang::DeclRefExpr>("DeclRef")) {
        if (!DRE || !Result.Context->getSourceManager().isWrittenInMainFile(DRE->getLocation()))
            return;

        if (DRE->getType()->isUndeducedType()) {
            std::string returnType = "int";  // Ensure consistency with the function template
            SourceLocation StartLoc = DRE->getBeginLoc();
            SourceLocation EndLoc = DRE->getEndLoc();
            Rewrite.ReplaceText(SourceRange(StartLoc, EndLoc), returnType + " (*g)(int) = &f<int>");
        }
    }
}
  
void MutatorFrontendAction_47::MutatorASTConsumer_47::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher funcTemplateMatcher = functionTemplateDecl(hasDescendant(functionDecl(returns(autoType())))).bind("FuncTemplate");
    StatementMatcher declRefExprMatcher = declRefExpr(to(functionDecl(returns(autoType())))).bind("DeclRef");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(funcTemplateMatcher, &callback);
    matchFinder.addMatcher(declRefExprMatcher, &callback);
    matchFinder.matchAST(Context);
}