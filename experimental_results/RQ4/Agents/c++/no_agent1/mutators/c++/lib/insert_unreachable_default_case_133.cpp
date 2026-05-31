//source file
#include "../include/insert_unreachable_default_case_133.h"

// ========================================================================================================
#define MUT133_OUTPUT 1

void MutatorFrontendAction_133::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ST = Result.Nodes.getNodeAs<clang::SwitchStmt>("SwitchStmt")) {
        //Filter nodes in header files
        if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                       ST->getBeginLoc()))
            return;

        //Get the source code text of target node
        auto switchText = stringutils::rangetoStr(*(Result.SourceManager),
                                                  ST->getSourceRange());

        //Perform mutation on the source code text by applying string replacement
        size_t pos = switchText.rfind('}');
        if (pos != std::string::npos) {
            std::string defaultCase = "\n/*mut133*/default: __builtin_unreachable();\n";
            switchText.insert(pos, defaultCase);
        }

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(ST->getSourceRange()), switchText);
    }
}

void MutatorFrontendAction_133::MutatorASTConsumer_133::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation
    StatementMatcher matcher = switchStmt().bind("SwitchStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}