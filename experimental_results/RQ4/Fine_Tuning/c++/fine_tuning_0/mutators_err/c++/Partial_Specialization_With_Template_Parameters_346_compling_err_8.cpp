//header file
#pragma once
#include "Mutator_base.h"

/**
 * Partial_Specialization_With_Template_Parameters_346
 */ 
class MutatorFrontendAction_346 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(346)

private:
    class MutatorASTConsumer_346 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_346(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Partial_Specialization_With_Template_Parameters_346.h"

// ========================================================================================================
#define MUT346_OUTPUT 1

void MutatorFrontendAction_346::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplatePartialSpecializationDecl>("PartialSpecialization")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto name = MT->getNameAsString();
      auto params = MT->getTemplateParameters();
      auto args = MT->getTemplateArgs();
      llvm::outs() << name << '\n';
      for (size_t i = 0; i < params->size(); ++i) {
        llvm::outs() << args->get(i).getAsType().getAsString() << '\n';
      }
      //Perform mutation on the source code text by applying string replacement
      std::string new_specialization = "template <";
      for (size_t i = 0; i < params->size(); ++i) {
        if (i)
          new_specialization += ", ";
        auto param = params->getParam(i);
        if (i == 0) {
          new_specialization += args->get(i).getAsType().getAsString();
        } else {
          new_specialization += param->getNameAsString();
        }
      }
      new_specialization += "> struct ";
      new_specialization += name;
      new_specialization += "<";
      for (size_t i = 0; i < params->size(); ++i) {
        if (i)
          new_specialization += ", ";
        auto param = params->getParam(i);
        new_specialization += param->getNameAsString();
      }
      new_specialization += ">";
      llvm::outs() << new_specialization << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getBeginLoc(), name.size(), new_specialization);
    }
}
  
void MutatorFrontendAction_346::MutatorASTConsumer_346::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplatePartialSpecializationDecl().bind("PartialSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}