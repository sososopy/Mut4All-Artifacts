//source file
#include "../include/EnumWithIncompleteUnderlyingType_435.h"

// ========================================================================================================
#define MUT435_OUTPUT 1

void MutatorFrontendAction_435::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("enumDecl")) {
        // Filter nodes in header files
        if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                       ED->getLocation()))
            return;

        // Insert forward declaration before the enum
        SourceLocation insertLoc = ED->getLocation();
        Rewrite.InsertText(insertLoc, "struct NewType_435;\n", true, true);

        // Modify the enum to use the new underlying type
        std::string enumText = stringutils::rangetoStr(*Result.SourceManager, ED->getSourceRange());
        size_t openBracePos = enumText.find('{');
        if (openBracePos != std::string::npos) {
            enumText.insert(openBracePos, " : NewType_435");
            Rewrite.ReplaceText(ED->getSourceRange(), enumText);
        }
    }
}
  
void MutatorFrontendAction_435::MutatorASTConsumer_435::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatcher to find enum declarations without an explicit underlying type
    DeclarationMatcher matcher = enumDecl().bind("enumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}