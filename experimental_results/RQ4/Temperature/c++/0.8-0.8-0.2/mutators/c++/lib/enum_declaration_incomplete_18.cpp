//source file
#include "../include/enum_declaration_incomplete_18.h"

// ========================================================================================================
#define MUT18_OUTPUT 1

void MutatorFrontendAction_18::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("enumDecl")) {
      //Filter nodes in header files
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;

      // Remove the list of enumerators and add a semicolon after the enum name
      SourceLocation startLoc = ED->getBeginLoc();
      SourceLocation endLoc = ED->getSourceRange().getEnd().getLocWithOffset(1);
      
      //Get the source code text of target node
      std::string enumText = "enum " + ED->getNameAsString() + ";";
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(startLoc, endLoc), enumText);
    }
}

void MutatorFrontendAction_18::MutatorASTConsumer_18::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumDecl(isExpansionInMainFile()).bind("enumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}