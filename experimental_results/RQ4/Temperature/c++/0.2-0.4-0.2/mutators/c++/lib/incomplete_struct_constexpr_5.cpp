//source file
#include "../include/incomplete_struct_constexpr_5.h"

// ========================================================================================================
#define MUT5_OUTPUT 1

void MutatorFrontendAction_5::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *StructDecl = Result.Nodes.getNodeAs<clang::RecordDecl>("IncompleteStruct")) {
        // Filter nodes in header files
        if (!StructDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                StructDecl->getLocation()))
            return;
        // Ensure the struct is incomplete
        if (StructDecl->isCompleteDefinition())
            return;
        incompleteStructDecl = StructDecl;
    } else if (auto *ConstexprVar = Result.Nodes.getNodeAs<clang::VarDecl>("ConstexprVar")) {
        if (!ConstexprVar || !Result.Context->getSourceManager().isWrittenInMainFile(
                ConstexprVar->getLocation()))
            return;
        if (!incompleteStructDecl)
            return;

        // Get the source code text of target node
        std::string varDeclText = stringutils::rangetoStr(*(Result.SourceManager), ConstexprVar->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        std::string mutatedText = "constexpr struct " + incompleteStructDecl->getNameAsString() + " " + ConstexprVar->getNameAsString() + " = { 0 };";

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(ConstexprVar->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_5::MutatorASTConsumer_5::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher structMatcher = recordDecl(unless(isDefinition())).bind("IncompleteStruct");
    DeclarationMatcher constexprMatcher = varDecl(hasType(isInteger())).bind("ConstexprVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(structMatcher, &callback);
    matchFinder.addMatcher(constexprMatcher, &callback);
    matchFinder.matchAST(Context);
}