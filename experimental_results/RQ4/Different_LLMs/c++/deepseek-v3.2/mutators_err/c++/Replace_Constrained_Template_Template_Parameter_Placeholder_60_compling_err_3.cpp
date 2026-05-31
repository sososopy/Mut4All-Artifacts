//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Constrained_Template_Template_Parameter_Placeholder_60
 */ 
class MutatorFrontendAction_60 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(60)

private:
    class MutatorASTConsumer_60 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_60(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> candidateTypes = {"int", "char", "short", "long", "long long"};
    };
};

//source file
#include "../include/Mutator_Replace_Constrained_Template_Template_Parameter_Placeholder_60.h"

// ========================================================================================================
#define MUT60_OUTPUT 1

void MutatorFrontendAction_60::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TP = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("templateTemplateParam")) {
      //Filter nodes in header files
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      
      // Check if this is a template template parameter with a constrained placeholder
      auto *templateDecl = dyn_cast<TemplateDecl>(TP->getDeclContext());
      if (!templateDecl) return;
      
      // Get the source range of the template template parameter
      SourceRange range = TP->getSourceRange();
      if (!range.isValid()) return;
      
      // Get the source code text of target node
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), range);
      
      // Look for constrained auto placeholder pattern
      size_t autoPos = originalText.find("auto");
      if (autoPos == std::string::npos) return;
      
      // Check if there's a concept constraint before auto
      size_t conceptPos = originalText.rfind(" ", autoPos - 1);
      if (conceptPos == std::string::npos) return;
      
      // Extract potential concept name
      std::string beforeAuto = originalText.substr(0, autoPos);
      size_t lastSpace = beforeAuto.find_last_of(" ");
      if (lastSpace == std::string::npos) return;
      
      std::string possibleConcept = beforeAuto.substr(lastSpace + 1);
      
      // Choose a replacement type
      std::string replacementType = candidateTypes[getrandom::getRandomIndex(candidateTypes.size() - 1)];
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = originalText;
      mutatedText.replace(autoPos, 4, replacementType);
      
      // Also replace any other auto placeholders in the same parameter list if they exist
      size_t nextAutoPos = mutatedText.find("auto", autoPos + replacementType.length());
      while (nextAutoPos != std::string::npos) {
        // Check if this auto is also constrained by the same concept pattern
        size_t checkPos = mutatedText.rfind(" ", nextAutoPos - 1);
        if (checkPos != std::string::npos) {
          std::string beforeThisAuto = mutatedText.substr(0, nextAutoPos);
          size_t thisLastSpace = beforeThisAuto.find_last_of(" ");
          if (thisLastSpace != std::string::npos) {
            std::string thisConcept = beforeThisAuto.substr(thisLastSpace + 1);
            if (thisConcept == possibleConcept) {
              mutatedText.replace(nextAutoPos, 4, replacementType);
            }
          }
        }
        nextAutoPos = mutatedText.find("auto", nextAutoPos + replacementType.length());
      }
      
      // Add mutation comment
      mutatedText = "/*mut60*/" + mutatedText;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(range), mutatedText);
    }
}
  
void MutatorFrontendAction_60::MutatorASTConsumer_60::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateTypeParmDecl(
        hasAncestor(decl().bind("templateDecl")),
        hasType(autoType())
    ).bind("templateTemplateParam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}