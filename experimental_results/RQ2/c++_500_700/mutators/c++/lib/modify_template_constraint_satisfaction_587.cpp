//source file
#include "../include/modify_template_constraint_satisfaction_587.h"

// ========================================================================================================
#define MUT587_OUTPUT 1

void MutatorFrontendAction_587::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (auto *RC = FD->getTemplateParameters()->getRequiresClause()) {
            std::string newConcept = "new_concept";
            std::string condition = "/* some condition on T */";

            std::string newConceptDecl = "template<typename T>\nconcept " + newConcept + " = " + condition + ";\n";
            Rewrite.InsertText(FD->getBeginLoc(), newConceptDecl);

            std::string originalConstraint = Lexer::getSourceText(CharSourceRange::getTokenRange(RC->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts()).str();
            std::string newConstraint = "(" + originalConstraint + " && " + newConcept + "<U>)";
            Rewrite.ReplaceText(RC->getSourceRange(), newConstraint);
        }
    }
}

void MutatorFrontendAction_587::MutatorASTConsumer_587::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}