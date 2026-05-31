//source file
#include "../include/Inline_Assembly_Syntax_Alteration_272.h"

// ========================================================================================================
#define MUT272_OUTPUT 1

void MutatorFrontendAction_272::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *AS = Result.Nodes.getNodeAs<clang::AsmStmt>("asmStmt")) {
        // Filter nodes in header files
        if (!AS || !Result.Context->getSourceManager().isWrittenInMainFile(AS->getBeginLoc()))
            return;

        // Ensure asm has at least one output or input operand
        unsigned numOutput = AS->getNumOutputs();
        unsigned numInput = AS->getNumInputs();
        if (numOutput == 0 && numInput == 0) return;

        // Get source text of the asm statement
        SourceLocation start = AS->getBeginLoc();
        SourceLocation end = AS->getEndLoc();
        CharSourceRange csr = CharSourceRange::getTokenRange(start, end);
        SourceRange sr = csr.getAsRange();
        std::string originalText = stringutils::rangetoStr(*Result.SourceManager, sr);

        // Insert trailing colon before closing parenthesis
        size_t closingParenPos = originalText.find_last_of(')');
        if (closingParenPos != std::string::npos) {
            originalText.insert(closingParenPos, ":");
        }

        // Replace original text with mutated version
        Rewrite.ReplaceText(csr, originalText);
    }
}

void MutatorFrontendAction_272::MutatorASTConsumer_272::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    clang::ast_matchers::StatementMatcher matcher = clang::ast_matchers::asmStmt().bind("asmStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}