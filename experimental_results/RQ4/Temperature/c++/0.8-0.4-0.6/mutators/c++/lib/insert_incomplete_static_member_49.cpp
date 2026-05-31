//source file
#include "../include/insert_incomplete_static_member_49.h"

// ========================================================================================================
#define MUT49_OUTPUT 1

void MutatorFrontendAction_49::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *AnonStruct = Result.Nodes.getNodeAs<clang::RecordDecl>("AnonStruct")) {
        if (!AnonStruct || !Result.Context->getSourceManager().isWrittenInMainFile(
                AnonStruct->getLocation()))
            return;

        if (!AnonStruct->isAnonymousStructOrUnion())
            return;

        auto structRange = AnonStruct->getSourceRange();
        std::string structText = stringutils::rangetoStr(*(Result.SourceManager), structRange);

        std::string incompleteTypeDecl = "\nstatic ::IncompleteType;\n";
        if (structText.rfind('}') != std::string::npos)
            structText.insert(structText.rfind('}'), incompleteTypeDecl);

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(structRange), structText);
    }
}

void MutatorFrontendAction_49::MutatorASTConsumer_49::HandleTranslationUnit(ASTContext &Context) {
    using namespace clang::ast_matchers;
    MatchFinder matchFinder;
    DeclarationMatcher matcher = recordDecl(hasName("")).bind("AnonStruct"); // Changed to use hasName("") as a workaround
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}