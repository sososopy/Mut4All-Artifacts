//header file
#pragma once
#include "Mutator_base.h"

/**
 * Dependent_Function_Call_In_Non_Type_Template_Argument_473
 */ 
class MutatorFrontendAction_473 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(473)

private:
    class MutatorASTConsumer_473 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_473(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl*> templateDecls;
    };
};

//source file
#include "../include/Dependent_Function_Call_In_Non_Type_Template_Argument_473.h"

// ========================================================================================================
#define MUT473_OUTPUT 1

void MutatorFrontendAction_473::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto templateParams = MT->getTemplateParameters();
      bool hasAutoParam = false;
      for (auto param : *templateParams) {
        if (auto *NTTP = dyn_cast<clang::NonTypeTemplateParmDecl>(param)) {
          if (NTTP->getType().getAsString().find("auto") != std::string::npos) {
            hasAutoParam = true;
            break;
          }
        }
      }
      if (!hasAutoParam) return;
      //(optional)Record the node information to be used in the mutation process
      templateDecls.push_back(MT);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("Specialization")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocStart()))
        return;
      //Get the source code text of target node
      auto templateDecl = MT->getTemplateName().getAsTemplateDecl();
      if (!templateDecl) return;
      //Check if this specialization uses one of the recorded template declarations
      bool isTarget = false;
      for (auto td : templateDecls) {
        if (td == templateDecl) {
          isTarget = true;
          break;
        }
      }
      if (!isTarget) return;
      
      //Perform mutation on the source code text by applying string replacement
      //Create a placeholder struct T with static member value
      std::string placeholderStruct = "struct Mut_473_T { static int value; };";
      //Create a functor
      std::string functor = "struct Mut_473_Functor { void operator()(int){}; } Mut_473_f;";
      //Create alias declaration with dependent function call
      std::string aliasDecl = "template<typename T> using Mut_473_Alias = ";
      aliasDecl += templateDecl->getNameAsString();
      aliasDecl += "<Mut_473_f(T::value)>;";
      
      //Combine all mutations
      std::string mutation = "/*mut473*/" + placeholderStruct + "\n" + functor + "\n" + aliasDecl;
      
      //Replace the original AST node with the mutated one
      SourceLocation insertLoc = MT->getLocStart();
      Rewrite.InsertTextBefore(insertLoc, mutation + "\n");
    }
}
  
void MutatorFrontendAction_473::MutatorASTConsumer_473::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher1 = classTemplateDecl().bind("TemplateDecl");
    TypeMatcher matcher2 = templateSpecializationType().bind("Specialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}