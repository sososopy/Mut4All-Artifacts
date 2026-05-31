//source file
#include "../include/replace_switch_with_if_else_434.h"

// ========================================================================================================
#define MUT434_OUTPUT 1

void MutatorFrontendAction_434::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ST = Result.Nodes.getNodeAs<clang::SwitchStmt>("SwitchStmt")) {
        if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(ST->getBeginLoc()))
            return;

        std::string ifElseCode;
        for (auto *Child : ST->getBody()->children()) {
            if (auto *Case = llvm::dyn_cast<clang::CaseStmt>(Child)) {
                auto caseCond = stringutils::rangetoStr(*(Result.SourceManager), Case->getLHS()->getSourceRange());
                ifElseCode += "if (" + stringutils::rangetoStr(*(Result.SourceManager), ST->getCond()->getSourceRange()) + " == " + caseCond + ") {\n";
                for (auto *SubChild : Case->getSubStmt()->children()) {
                    ifElseCode += stringutils::rangetoStr(*(Result.SourceManager), SubChild->getSourceRange()) + "\n";
                }
                ifElseCode += "}\n";
            }
            else if (auto *Default = llvm::dyn_cast<clang::DefaultStmt>(Child)) {
                ifElseCode += "else {\n";
                for (auto *SubChild : Default->getSubStmt()->children()) {
                    ifElseCode += stringutils::rangetoStr(*(Result.SourceManager), SubChild->getSourceRange()) + "\n";
                }
                ifElseCode += "}\n";
            }
        }
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(ST->getSourceRange()), ifElseCode);
    }
}

void MutatorFrontendAction_434::MutatorASTConsumer_434::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = switchStmt().bind("SwitchStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}