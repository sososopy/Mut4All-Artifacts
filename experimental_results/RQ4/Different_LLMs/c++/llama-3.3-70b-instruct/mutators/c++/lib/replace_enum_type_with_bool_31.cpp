//source file
#include "../include/Replace_Enum_Type_With_Bool_31.h"

// ========================================================================================================
#define MUT31_OUTPUT 1

void MutatorFrontendAction_31::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ET = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumType")) {
      //Filter nodes in header files
      if (!ET || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ET->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   ET->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      size_t pos = declaration.find("enum");
      if (pos != string::npos) {
        declaration.replace(pos, 4, "bool");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ET->getSourceRange()), declaration);
    }
}

void MutatorFrontendAction_31::MutatorASTConsumer_31::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumDecl().bind("EnumType");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}