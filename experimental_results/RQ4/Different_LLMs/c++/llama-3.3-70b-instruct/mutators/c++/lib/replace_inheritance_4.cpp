//source file
#include "../include/Replace_Inheritance_4.h"

// ========================================================================================================
#define REPLACE_INHERITANCE_4_OUTPUT 1

void MutatorFrontendAction_4::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace the inheritance structure
      std::string mutatedDeclaration = declaration;
      // Check if the class has any base classes
      if (MT->getNumBases() > 0) {
        // Get the first base class
        auto baseClass = MT->bases_begin();
        // Replace the base class with a different one if available
        if (baseClass != MT->bases_end() && std::next(baseClass) != MT->bases_end()) {
          mutatedDeclaration.replace(mutatedDeclaration.find(baseClass->getType().getAsString()),
                                    baseClass->getType().getAsString().size(),
                                    std::next(baseClass)->getType().getAsString());
        }
      }
      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_4::MutatorASTConsumer_4::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(hasAnyBase(cxxBaseSpecifier())).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}