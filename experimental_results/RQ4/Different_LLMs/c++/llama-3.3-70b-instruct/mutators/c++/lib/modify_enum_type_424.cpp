//source file
#include "../include/Modify_Enum_Type_424.h"

// ========================================================================================================
#define MUT424_OUTPUT 1

void MutatorFrontendAction_424::Callback::run(const MatchFinder::MatchResult &Result) {
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
      std::string underlyingType = ET->getIntegerType()->getTypeClassName();
      if (underlyingType == "IntegerType") {
        declaration.replace(declaration.find("int"), 3, "char");
      } else if (underlyingType == "CharType") {
        declaration.replace(declaration.find("char"), 4, "short");
      } else if (underlyingType == "ShortType") {
        declaration.replace(declaration.find("short"), 5, "int");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ET->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_424::MutatorASTConsumer_424::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumDecl().bind("EnumType");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}