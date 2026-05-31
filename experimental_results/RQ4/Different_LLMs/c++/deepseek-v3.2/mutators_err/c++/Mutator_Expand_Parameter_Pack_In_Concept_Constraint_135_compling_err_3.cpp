//header file
#pragma once
#include "Mutator_base.h"

/**
 * Expand_Parameter_Pack_In_Concept_Constraint_135
 */ 
class MutatorFrontendAction_135 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(135)

private:
    class MutatorASTConsumer_135 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_135(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Expand_Parameter_Pack_In_Concept_Constraint_135.h"

// ========================================================================================================
#define MUT135_OUTPUT 1

void MutatorFrontendAction_135::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *NTTP = Result.Nodes.getNodeAs<clang::NonTypeTemplateParmDecl>("nonTypeParam")) {
      //Filter nodes in header files
      if (!NTTP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NTTP->getLocation()))
        return;
      //Get the source code text of target node
      auto paramRange = NTTP->getSourceRange();
      auto paramText = stringutils::rangetoStr(*(Result.SourceManager), paramRange);
      
      //Check if this non-type template parameter has a type constraint
      if (NTTP->hasDefaultArgument() || NTTP->hasTypeConstraint()) {
        //Get the constraint expression
        auto *C = NTTP->getTypeConstraint();
        if (C) {
          //Check if constraint is a concept specialization
          if (auto *CR = dyn_cast<ConceptSpecializationExpr>(C->getImmediatelyDeclaredConstraint())) {
            auto *Concept = CR->getNamedConcept();
            auto Args = CR->getTemplateArgs();
            
            //Look for unexpanded parameter packs in the concept arguments
            bool foundPack = false;
            std::string newText = paramText;
            
            //We need to find the location of the unexpanded pack in the source
            //We'll search for the pack name in the constraint part
            std::size_t constraintPos = newText.find(Concept->getNameAsString());
            if (constraintPos != std::string::npos) {
              //Find the template argument list after the concept name
              std::size_t argStart = newText.find('<', constraintPos);
              if (argStart != std::string::npos) {
                std::size_t argEnd = newText.find('>', argStart);
                if (argEnd != std::string::npos) {
                  std::string argList = newText.substr(argStart + 1, argEnd - argStart - 1);
                  
                  //Check each template parameter pack that might appear in arguments
                  auto *TPL = NTTP->getTemplateParameterList(0);
                  if (TPL) {
                    for (auto *Param : *TPL) {
                      if (auto *TTP = dyn_cast<TemplateTypeParmDecl>(Param)) {
                        if (TTP->isParameterPack()) {
                          std::string packName = TTP->getNameAsString();
                          //Look for this pack name in the argument list without "..."
                          std::size_t packPos = argList.find(packName);
                          if (packPos != std::string::npos) {
                            //Check if it's already expanded (has ... after it)
                            std::size_t afterPack = packPos + packName.length();
                            if (afterPack < argList.length() && 
                                argList.substr(afterPack, 3) != "...") {
                              //This is an unexpanded pack - expand it
                              std::string newArgList = argList;
                              newArgList.insert(afterPack, "...");
                              newText.replace(argStart + 1, argEnd - argStart - 1, newArgList);
                              foundPack = true;
                              break;
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
            
            //Perform mutation on the source code text by applying string replacement
            if (foundPack) {
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(paramRange), newText);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_135::MutatorASTConsumer_135::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = nonTypeTemplateParmDecl(
      unless(hasAncestor(functionDecl()))
    ).bind("nonTypeParam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}