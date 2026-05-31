//source file
#include "../include/union_in_class_initialization_23.h"

// ========================================================================================================
#define MUT23_OUTPUT 1

void MutatorFrontendAction_23::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UD = Result.Nodes.getNodeAs<clang::RecordDecl>("UnionDecl")) {
        if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(UD->getLocation()))
            return;

        if (UD->isUnion()) {
            for (auto *Field : UD->fields()) {
                auto TypeName = Field->getType().getAsString();
                std::string MutatedCode = TypeName + " " + Field->getNameAsString() + " = ;";
                Rewrite.ReplaceText(Field->getSourceRange(), MutatedCode);
            }
        }
    }
}
  
void MutatorFrontendAction_23::MutatorASTConsumer_23::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = recordDecl(isUnion()).bind("UnionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}