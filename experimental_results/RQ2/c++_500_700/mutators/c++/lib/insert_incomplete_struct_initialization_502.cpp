//source file
#include "../include/insert_incomplete_struct_initialization_502.h"

// ========================================================================================================
#define MUT502_OUTPUT 1

void MutatorFrontendAction_502::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SD = Result.Nodes.getNodeAs<clang::RecordDecl>("StructDecl")) {
        if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(SD->getLocation()))
            return;
        if (SD->isCompleteDefinition())
            return;

        std::string structName = SD->getNameAsString();
        std::string newVarDecl = "constexpr struct " + structName + " " + structName + "_instance = { 0 }; /*mut502*/";
        SourceLocation insertLoc = SD->getEndLoc().getLocWithOffset(1);

        Rewrite.InsertText(insertLoc, "\n" + newVarDecl);
    }
}

void MutatorFrontendAction_502::MutatorASTConsumer_502::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = recordDecl(unless(isDefinition())).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}