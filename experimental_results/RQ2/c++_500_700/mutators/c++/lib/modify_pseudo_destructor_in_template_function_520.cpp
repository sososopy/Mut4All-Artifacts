//source file
#include "../include/modify_pseudo_destructor_in_template_function_520.h"

// ========================================================================================================
#define MUT520_OUTPUT 1

void MutatorFrontendAction_520::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Method = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("templateMethod")) {
        if (!Method || !Result.Context->getSourceManager().isWrittenInMainFile(Method->getLocation()))
            return;

        if (Method->hasBody()) {
            auto body = Method->getBody();
            for (auto stmt : body->children()) {
                if (auto *pseudoDtorExpr = dyn_cast<clang::CXXPseudoDestructorExpr>(stmt)) {
                    std::string mutatedExpr = "(*ptr).~int()";
                    Rewrite.ReplaceText(pseudoDtorExpr->getSourceRange(), mutatedExpr);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_520::MutatorASTConsumer_520::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(ofClass(cxxRecordDecl(isTemplateInstantiation()))).bind("templateMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}