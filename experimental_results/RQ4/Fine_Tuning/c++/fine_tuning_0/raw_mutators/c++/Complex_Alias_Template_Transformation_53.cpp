//header file
#pragma once
#include "Mutator_base.h"

/**
 * Complex_Alias_Template_Transformation_53
 */ 
class MutatorFrontendAction_53 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(53)

private:
    class MutatorASTConsumer_53 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_53(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Complex_Alias_Template_Transformation_53.h"

// ========================================================================================================
#define MUT53_OUTPUT 1

void MutatorFrontendAction_53::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("AliasTemplate")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto aliastemplate = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      llvm::outs() << aliastemplate << "\n";
      auto alias = MT->getTemplatedDecl();
      auto aliastype = alias->getUnderlyingType();
      auto aliasstr = aliastype.getAsString();
      llvm::outs() << aliasstr << "\n";
      //Perform mutation on the source code text by applying string replacement
      if (aliastype->isTemplateSpecializationType()) {
        auto template_specialization_type =
            aliastype->getAs<TemplateSpecializationType>();
        auto template_decl = template_specialization_type->getTemplateName()
                                 .getAsTemplateDecl();
        auto template_name = template_decl->getNameAsString();
        auto template_args = template_specialization_type->template_arguments();
        for (auto arg : template_args) {
          if (arg.getKind() == TemplateArgument::Type) {
            auto arg_type = arg.getAsType();
            if (arg_type->isDependentType()) {
              aliasstr = "typename " + aliasstr + "::type";
            }
          }
        }
      }
      llvm::outs() << aliasstr << "\n";
      aliastemplate = "/*mut53*/" + aliastemplate;
      aliastemplate.replace(aliastemplate.find_last_of('=') + 1,
                            std::string::npos, aliasstr);
      llvm::outs() << aliastemplate << "\n";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), aliastemplate);
    }
}
  
void MutatorFrontendAction_53::MutatorASTConsumer_53::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasTemplateDecl().bind("AliasTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}