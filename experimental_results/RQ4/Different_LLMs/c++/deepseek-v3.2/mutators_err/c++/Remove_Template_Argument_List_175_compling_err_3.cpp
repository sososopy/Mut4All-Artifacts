//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Template_Argument_List_175
 */ 
class MutatorFrontendAction_175 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(175)

private:
    class MutatorASTConsumer_175 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_175(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Remove_Template_Argument_List_175.h"

// ========================================================================================================
#define MUT175_OUTPUT 1

void MutatorFrontendAction_175::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::DependentTemplateSpecializationType>("templateSpecialization")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocStart()))
        return;
      //Get the source code text of target node
      auto sourceRange = MT->getSourceRange();
      auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      //Perform mutation on the source code text by applying string replacement
      size_t templatePos = sourceText.find("template ");
      if (templatePos != std::string::npos) {
        size_t angleBracketStart = sourceText.find('<', templatePos);
        if (angleBracketStart != std::string::npos) {
          size_t angleBracketEnd = sourceText.rfind('>');
          if (angleBracketEnd != std::string::npos && angleBracketEnd > angleBracketStart) {
            sourceText.erase(angleBracketStart, angleBracketEnd - angleBracketStart + 1);
            sourceText = "/*mut175*/" + sourceText;
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), sourceText);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_175::MutatorASTConsumer_175::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateSpecializationType().bind("templateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}