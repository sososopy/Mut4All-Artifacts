//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Constrained_Variable_Template_357
 */ 
class MutatorFrontendAction_357 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(357)

private:
    class MutatorASTConsumer_357 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_357(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Constrained_Variable_Template_357.h"

// ========================================================================================================
#define MUT357_OUTPUT 1

void MutatorFrontendAction_357::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarTemplateDecl>("VarTemplateDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Get the source code text of target node
      auto sourceRange = VD->getSourceRange();
      auto originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      
      //Check if it's a constrained variable template
      auto *TemplDecl = VD->getDescribedTemplate();
      if (!TemplDecl) return;
      
      //Check for constraints: requires clause or constrained template parameters
      bool hasConstraint = false;
      // Check for requires clause using TemplateDecl's getRequiresClause
      if (TemplDecl->getASTContext().getLangOpts().CPlusPlus20) {
        // Use getTemplateParameters to check constraints
        auto *TPL = TemplDecl->getTemplateParameters();
        if (TPL) {
          for (auto *Param : TPL->asArray()) {
            if (Param->isParameterPack()) continue;
            // Check if parameter has a constraint
            if (auto *TTP = dyn_cast<TemplateTypeParmDecl>(Param)) {
                if (TTP->hasDefaultArgument() && TTP->getDefaultArgument()) {
                    hasConstraint = true;
                    break;
                }
            }
          }
        }
      }
      
      if (!hasConstraint) return;
      
      //Perform mutation on the source code text by applying string replacement
      //Remove constraint and introduce invalid syntax
      std::string mutatedText = originalText;
      
      //Find template keyword and parameter list
      size_t templatePos = mutatedText.find("template");
      if (templatePos == std::string::npos) return;
      
      //Insert invalid syntax: duplicate template keyword with stray character
      size_t afterTemplate = mutatedText.find('<', templatePos);
      if (afterTemplate == std::string::npos) return;
      
      //Insert invalid duplicate template declaration with stray apostrophe
      mutatedText.insert(afterTemplate, "template<typename T'> ");
      
      //Remove requires clause if present
      size_t requiresPos = mutatedText.find("requires");
      if (requiresPos != std::string::npos) {
        size_t requiresEnd = mutatedText.find(';', requiresPos);
        if (requiresEnd == std::string::npos) requiresEnd = mutatedText.length();
        size_t clauseEnd = mutatedText.find_first_of("\n;{", requiresPos);
        if (clauseEnd != std::string::npos) {
          mutatedText.erase(requiresPos, clauseEnd - requiresPos);
        }
      }
      
      //Replace constrained template parameters with plain typename
      size_t paramStart = mutatedText.find('<', templatePos);
      size_t paramEnd = mutatedText.find('>', paramStart);
      if (paramStart != std::string::npos && paramEnd != std::string::npos) {
        std::string paramText = mutatedText.substr(paramStart, paramEnd - paramStart + 1);
        //Replace concept names with typename
        //Simple replacement: any word that might be a concept becomes typename
        size_t conceptPos = paramText.find("Concept");
        if (conceptPos != std::string::npos) {
          paramText.replace(conceptPos, 7, "typename");
          mutatedText.replace(paramStart, paramEnd - paramStart + 1, paramText);
        }
      }
      
      mutatedText = "/*mut357*/" + mutatedText;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}
  
void MutatorFrontendAction_357::MutatorASTConsumer_357::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varTemplateDecl().bind("VarTemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}