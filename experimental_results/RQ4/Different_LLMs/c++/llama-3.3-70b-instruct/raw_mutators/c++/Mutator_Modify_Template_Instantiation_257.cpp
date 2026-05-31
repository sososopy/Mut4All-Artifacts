//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Instantiation_257
 */ 
class MutatorFrontendAction_257 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(257)

private:
    class MutatorASTConsumer_257 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_257(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Modify_Template_Instantiation_257.h"

// ========================================================================================================
#define MUT257_OUTPUT 1

void MutatorFrontendAction_257::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateInstantiation")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // For example, modify the template instantiation by changing the number or types of template arguments
      std::string mutatedDeclaration = declaration;
      // Replace the original template arguments with new ones
      size_t pos = mutatedDeclaration.find('<');
      if (pos != std::string::npos) {
        size_t endPos = mutatedDeclaration.find('>');
        if (endPos != std::string::npos) {
          std::string templateArgs = mutatedDeclaration.substr(pos + 1, endPos - pos - 1);
          // Modify the template arguments
          templateArgs += ", int"; // Add a new template argument
          mutatedDeclaration.replace(pos + 1, endPos - pos - 1, templateArgs);
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_257::MutatorASTConsumer_257::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateInstantiation");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}