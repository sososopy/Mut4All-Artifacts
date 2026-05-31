//source file
#include "../include/insert_unreachable_code_block_104.h"

// ========================================================================================================
#define MUT104_OUTPUT 1

void MutatorFrontendAction_104::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FS->getLocation()))
            return;

        if (FS->hasBody()) {
            auto functionBody = stringutils::rangetoStr(*(Result.SourceManager),
                                                        FS->getBody()->getSourceRange());
            size_t insertPos = functionBody.find_last_of("}");
            if (insertPos != std::string::npos) {
                std::string unreachableCode = "\n/*mut104*/if (false) { throw \"Unreachable code executed!\"; }\n";
                functionBody.insert(insertPos, unreachableCode);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(FS->getSourceRange()), functionBody);
            }
        }
    }
}

void MutatorFrontendAction_104::MutatorASTConsumer_104::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}