//source file
#include "../include/nested_struct_redefinition_589.h"

// ========================================================================================================
#define MUT589_OUTPUT 1

void MutatorFrontendAction_589::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SD = Result.Nodes.getNodeAs<clang::RecordDecl>("StructDecl")) {
        if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       SD->getLocation()))
            return;

        if (SD->isStruct() && SD->isCompleteDefinition()) {
            auto structSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                        SD->getSourceRange());
            auto structName = SD->getNameAsString();
            std::string newStruct = "struct " + structName + " { float b }"; // No semicolon to simulate an error
            structSource += "\n/*mut589*/" + newStruct;
            Rewrite.ReplaceText(SD->getSourceRange(), structSource);
        }
    }
}
  
void MutatorFrontendAction_589::MutatorASTConsumer_589::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = recordDecl(isStruct(), isDefinition(), isExpansionInMainFile()).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}