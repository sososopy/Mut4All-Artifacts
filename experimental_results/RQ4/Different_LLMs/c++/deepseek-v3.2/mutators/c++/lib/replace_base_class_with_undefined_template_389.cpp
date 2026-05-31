//source file
#include "../include/Replace_Base_Class_With_Undefined_Template_389.h"

// ========================================================================================================
#define MUT389_OUTPUT 1

void MutatorFrontendAction_389::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      if (!MT->isCompleteDefinition() || MT->getNumBases() == 0)
        return;
      
      //Check for existing template declarations
      if (!hasUndefTemplateDecl) {
        //Look for any template class declarations
        for (const auto &decl : Result.Context->getTranslationUnitDecl()->decls()) {
          if (auto *templateDecl = dyn_cast<ClassTemplateDecl>(decl)) {
            if (templateDecl->getTemplatedDecl()->getNameAsString() == "UndefTemplate") {
              hasUndefTemplateDecl = true;
              break;
            }
          }
        }
        //If not found, add a forward declaration
        if (!hasUndefTemplateDecl) {
          SourceLocation insertLoc = MT->getBeginLoc();
          Rewrite.InsertTextBefore(insertLoc, "template <int> class UndefTemplate;\n");
          hasUndefTemplateDecl = true;
        }
      }
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = MT->getNameAsString() + " : public UndefTemplate<1>";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_389::MutatorASTConsumer_389::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}