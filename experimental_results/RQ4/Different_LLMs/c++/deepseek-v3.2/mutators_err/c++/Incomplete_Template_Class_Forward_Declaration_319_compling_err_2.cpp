//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incomplete_Template_Class_Forward_Declaration_319
 */ 
class MutatorFrontendAction_319 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(319)

private:
    class MutatorASTConsumer_319 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_319(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl*> templateDefinitions;
    };
};

//source file
#include "../include/Incomplete_Template_Class_Forward_Declaration_319.h"

// ========================================================================================================
#define MUT319_OUTPUT 1

void MutatorFrontendAction_319::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ForwardDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Check if it's a forward declaration (incomplete)
      if (FD->isThisDeclarationADefinition())
        return;
      
      //Get the source code text of target node
      auto forwardDeclText = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
      
      //Find corresponding definition
      const clang::ClassTemplateDecl* matchingDef = nullptr;
      for (const auto* def : templateDefinitions) {
        if (def->getNameAsString() == FD->getNameAsString()) {
          matchingDef = def;
          break;
        }
      }
      
      if (!matchingDef || !matchingDef->isThisDeclarationADefinition())
        return;
      
      //Get template parameters from definition
      auto* defTP = matchingDef->getTemplateParameters();
      if (!defTP)
        return;
      
      unsigned defParamCount = defTP->size();
      
      //Get template parameters from forward declaration
      auto* fdTP = FD->getTemplateParameters();
      unsigned fdParamCount = fdTP ? fdTP->size() : 0;
      
      //Only mutate if forward declaration exists and has different parameter count
      if (fdParamCount == defParamCount)
        return;
      
      //Perform mutation on the source code text by applying string replacement
      //We'll create an incorrect forward declaration by altering template arguments
      std::string mutatedDecl;
      
      //Build template parameter list
      mutatedDecl += "template <";
      for (unsigned i = 0; i < fdParamCount; ++i) {
        if (i > 0) mutatedDecl += ", ";
        auto* param = fdTP->getParam(i);
        if (auto* ttp = dyn_cast<clang::TemplateTypeParmDecl>(param)) {
          if (ttp->wasDeclaredWithTypename())
            mutatedDecl += "typename ";
          else
            mutatedDecl += "class ";
          mutatedDecl += ttp->getNameAsString();
        } else if (auto* nttp = dyn_cast<clang::NonTypeTemplateParmDecl>(param)) {
          mutatedDecl += nttp->getType().getAsString() + " ";
          mutatedDecl += nttp->getNameAsString();
        } else if (auto* ttp = dyn_cast<clang::TemplateTemplateParmDecl>(param)) {
          mutatedDecl += "template <typename> class ";
          mutatedDecl += ttp->getNameAsString();
        }
      }
      mutatedDecl += ">";
      
      //Add class name with incorrect template arguments
      mutatedDecl += " class " + FD->getNameAsString();
      
      //Add extra/missing template arguments
      if (defParamCount > fdParamCount) {
        //Add extra arguments not in definition
        mutatedDecl += "<";
        for (unsigned i = 0; i < defParamCount; ++i) {
          if (i > 0) mutatedDecl += ", ";
          if (i < fdParamCount) {
            //Use existing parameter names
            auto* param = fdTP->getParam(i);
            mutatedDecl += param->getNameAsString();
          } else {
            //Add extra arguments (bool false/true)
            mutatedDecl += (getrandom::getRandomIndex(1) == 0) ? "false" : "true";
          }
        }
        mutatedDecl += ">";
      } else if (defParamCount < fdParamCount) {
        //Use fewer arguments than definition
        mutatedDecl += "<";
        for (unsigned i = 0; i < defParamCount; ++i) {
          if (i > 0) mutatedDecl += ", ";
          auto* param = fdTP->getParam(i);
          mutatedDecl += param->getNameAsString();
        }
        mutatedDecl += ">";
      }
      
      mutatedDecl += ";";
      mutatedDecl = "/*mut319*/" + mutatedDecl;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), mutatedDecl);
    }
    else if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDef")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
       2;
      //Record template definitions for later use
      if (TD->isThisDeclarationADefinition()) {
        templateDefinitions.push_back(TD);
      }
    }
}
  
void MutatorFrontendAction_319::MutatorASTConsumer_319::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    //Match forward declarations of template classes
    DeclarationMatcher forwardMatcher = classTemplateDecl(unless(isDefinition())).bind("ForwardDecl");
    //Match definitions of template classes
    DeclarationMatcher defMatcher = classTemplateDecl(isDefinition()).bind("TemplateDef");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(forwardMatcher, &callback);
    matchFinder.addMatcher(defMatcher, &callback);
    matchFinder.matchAST(Context);
}