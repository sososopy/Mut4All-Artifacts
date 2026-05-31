//source file
#include "../include/Mutator_Modify_Template_Specialization_146.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceManager.h"
#include <string>

// ========================================================================================================
#define MUT146_OUTPUT 1

void MutatorFrontendAction_146::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpecialization")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      // Replace the specialized type or value with a different one
      std::string newType = "double"; // Replace with a different type
      std::string declarationStr = declaration.str();
      size_t pos = declarationStr.find("int");
      if (pos != std::string::npos) {
        declarationStr.replace(pos, 3, newType);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declarationStr);
    }
}
  
void MutatorFrontendAction_146::MutatorASTConsumer_146::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}