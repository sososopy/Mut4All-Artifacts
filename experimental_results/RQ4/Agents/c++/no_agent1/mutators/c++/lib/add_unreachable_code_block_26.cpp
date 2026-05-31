//source file
#include "../include/add_unreachable_code_block_26.h"

// ========================================================================================================
#define MUT26_OUTPUT 1

void MutatorFrontendAction_26::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;
        
        if (FD->hasBody()) {
            auto bodyRange = FD->getBody()->getSourceRange();
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), bodyRange);
            
            // Perform mutation by adding an unreachable code block
            std::string unreachableCode = "\nif (false) {\n/*mut26*/ int unreachable_var = 42;\n}\n";
            bodyText.insert(bodyText.find("{") + 1, unreachableCode);
            
            // Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(bodyRange), bodyText);
        }
    }
}

void MutatorFrontendAction_26::MutatorASTConsumer_26::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}