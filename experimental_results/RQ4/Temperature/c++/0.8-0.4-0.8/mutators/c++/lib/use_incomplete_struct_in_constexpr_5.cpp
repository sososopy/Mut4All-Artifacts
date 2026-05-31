//source file
#include "../include/use_incomplete_struct_in_constexpr_5.h"

// ========================================================================================================
#define MUT5_OUTPUT 1

void MutatorFrontendAction_5::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SD = Result.Nodes.getNodeAs<clang::RecordDecl>("IncompleteStruct")) {
        if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(SD->getLocation()))
            return;

        if (SD->isCompleteDefinition())
            return;

        std::string structName = SD->getNameAsString();
        std::string constexprDecl = "constexpr " + structName + " myVar = {0};\n";

        SourceLocation insertLoc = SD->getEndLoc().getLocWithOffset(1);
        Rewrite.InsertTextAfter(insertLoc, "/*mut5*/" + constexprDecl);
    }
}

void MutatorFrontendAction_5::MutatorASTConsumer_5::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = recordDecl(unless(isDefinition())).bind("IncompleteStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}