//header file
#pragma once
#include "Mutator_base.h"

/**
 * Apply_CTAD_To_Template_Aliased_Type_With_CRTP_136
 */ 
class MutatorFrontendAction_136 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(136)

private:
    class MutatorASTConsumer_136 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_136(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Apply_CTAD_To_Template_Aliased_Type_With_CRTP_136.h"

// ========================================================================================================
#define MUT136_OUTPUT 1

void MutatorFrontendAction_136::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("Alias")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto aliased_type = MT->getUnderlyingType();
      if (auto *TST = aliased_type->getAs<clang::TemplateSpecializationType>()) {
        auto template_args = TST->getArgs();
        if (template_args.size() == 0)
          return;
        if (template_args[0].getKind() != clang::TemplateArgument::Type)
          return;
        auto first_arg_type = template_args[0].getAsType();
        if (first_arg_type->getAs<clang::TemplateTypeParmType>() == nullptr)
          return;
      } else
        return;
      auto alias_name = MT->getNameAsString();
      llvm::outs() << alias_name << '\n';
      //Perform mutation on the source code text by applying string replacement
      auto alias_name_without_template = alias_name.substr(
          0, alias_name.find('<')); // remove the template argument
      llvm::outs() << alias_name_without_template << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getBeginLoc(), alias_name.length(),
                          alias_name_without_template);
    }
}
  
void MutatorFrontendAction_136::MutatorASTConsumer_136::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasDecl().bind("Alias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}