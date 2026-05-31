//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Instantiation_367
 */ 
class MutatorFrontendAction_367 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(367)

private:
    class MutatorASTConsumer_367 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_367(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Template_Instantiation_367.h"

// ========================================================================================================
#define MUT367_OUTPUT 1

void MutatorFrontendAction_367::Callback::run(const MatchFinder::MatchResult &Result) {
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
      // Replace template arguments with different types
      std::string newDeclaration = declaration;
      size_t pos = newDeclaration.find('<');
      if (pos != std::string::npos) {
        size_t endPos = newDeclaration.find('>');
        if (endPos != std::string::npos) {
          std::string templateArgs = newDeclaration.substr(pos + 1, endPos - pos - 1);
          // Replace template arguments with new types (e.g., int, float, char)
          std::string newTemplateArgs = "float, char";
          newDeclaration.replace(pos + 1, endPos - pos - 1, newTemplateArgs);
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), newDeclaration);
    }
}
  
void MutatorFrontendAction_367::MutatorASTConsumer_367::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateInstantiation");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}