//source file
#include "../include/Replace_Inheritance_82.h"

// ========================================================================================================
#define REPLACE_INHERITANCE_82_OUTPUT 1

void MutatorFrontendAction_82::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("InheritedClass")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (MT->getNumBases() > 0) {
        auto base = MT->bases_begin();
        if (base->getAccessSpecifier() == AccessSpecifier::AS_public) {
          declaration.replace(declaration.find("public"), 6, "private");
        } else if (base->getAccessSpecifier() == AccessSpecifier::AS_private) {
          declaration.replace(declaration.find("private"), 7, "protected");
        } else if (base->getAccessSpecifier() == AccessSpecifier::AS_protected) {
          declaration.replace(declaration.find("protected"), 9, "public");
        } else {
          declaration.replace(declaration.find(":"), 1, "");
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_82::MutatorASTConsumer_82::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(hasAnyBase(anyOf(cxxBaseSpecifier(), cxxBaseSpecifier()))).bind("InheritedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}