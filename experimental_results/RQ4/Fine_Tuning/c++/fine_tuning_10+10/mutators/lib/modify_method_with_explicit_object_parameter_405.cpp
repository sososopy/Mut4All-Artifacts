//source file
#include "../include/modify_method_with_explicit_object_parameter_405.h"

// ========================================================================================================
#define MUT405_OUTPUT 1

void MutatorFrontendAction_405::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("MethodWithExplicitObject")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (MT->hasBody()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        declaration.insert(declaration.find("{") + 1, "\n/*mut405*/obj." + MT->getNameAsString() + "();\n");
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_405::MutatorASTConsumer_405::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(ofClass(cxxRecordDecl(isStruct(), isTemplateInstantiation())), 
                                               hasParameter(0, parmVarDecl(hasType(references(cxxRecordDecl(isStruct(), isTemplateInstantiation()))))))
                                               .bind("MethodWithExplicitObject");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}