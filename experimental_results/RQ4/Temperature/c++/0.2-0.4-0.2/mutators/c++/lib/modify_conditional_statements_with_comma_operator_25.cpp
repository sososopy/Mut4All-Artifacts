//source file
#include "../include/modify_conditional_statements_with_comma_operator_25.h"

// ========================================================================================================
#define MUT25_OUTPUT 1

void MutatorFrontendAction_25::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Cond = Result.Nodes.getNodeAs<clang::IfStmt>("conditionalStmt")) {
        if (!Cond || !Result.Context->getSourceManager().isWrittenInMainFile(
                Cond->getBeginLoc()))
            return;
        
        if (targetVarDecl) {
            auto varName = targetVarDecl->getNameAsString();
            auto condition = stringutils::rangetoStr(*(Result.SourceManager), Cond->getCond()->getSourceRange());
            std::string newCondition = "(" + varName + " = 0, " + condition + ")";
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(Cond->getCond()->getSourceRange()), newCondition);
        }
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                VD->getBeginLoc()))
            return;
        
        if (!targetVarDecl) {
            targetVarDecl = VD;
        }
    }
}
  
void MutatorFrontendAction_25::MutatorASTConsumer_25::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher conditionalMatcher = ifStmt().bind("conditionalStmt");
    DeclarationMatcher varDeclMatcher = varDecl().bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(conditionalMatcher, &callback);
    matchFinder.addMatcher(varDeclMatcher, &callback);
    matchFinder.matchAST(Context);
}