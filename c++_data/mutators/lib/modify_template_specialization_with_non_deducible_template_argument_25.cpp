//source file
#include "../include/Modify_Template_Specialization_With_Non_deducible_Template_Argument_25.h"

// ========================================================================================================
#define MUT25_OUTPUT 1

void MutatorFrontendAction_25::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 TD->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      size_t pos = declaration.find("typename = typename enable_if");
      if (pos != std::string::npos) {
          declaration.insert(pos, "typename U, ");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_25::MutatorASTConsumer_25::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(
        has(cxxMethodDecl(hasAnyParameter(hasType(isInteger())))),
        hasDescendant(typeLoc(loc(qualType(hasDeclaration(namedDecl(hasName("enable_if")))))))
    ).bind("TemplateDecl");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}