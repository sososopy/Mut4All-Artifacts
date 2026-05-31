//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Instantiation_with_Deduction_Guide_341
 */ 
class MutatorFrontendAction_341 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(341)

private:
    class MutatorASTConsumer_341 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_341(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Instantiation_with_Deduction_Guide_341.h"

// ========================================================================================================
#define MUT341_OUTPUT 1

void MutatorFrontendAction_341::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto class_name = MT->getNameAsString();
      auto params = MT->getTemplateParameters();
      std::string param_list = "";
      for (auto param : *params) {
        param_list += param->getNameAsString();
        param_list += ",";
      }
      if (param_list.size() > 0)
        param_list.pop_back();
      auto spec = "/*mut341*/" + class_name + "<" + param_list + ">";
      llvm::outs() << spec << '\n';
      //Perform mutation on the source code text by applying string replacement
      auto spec_ast = clang::ast_matchers::match(
          clang::ast_matchers::classTemplateSpecializationDecl(
              clang::ast_matchers::hasName(spec)),
          *MT->getTranslationUnitDecl(), *Result.Context);
      if (spec_ast.size() == 0)
        return;
      auto spec_decl = spec_ast[0]
                           .getNodeAs<clang::ClassTemplateSpecializationDecl>(
                               "ClassTemplateSpecialization");
      if (spec_decl == nullptr)
        return;
      auto spec_def =
          stringutils::rangetoStr(*(Result.SourceManager), spec_decl->getSourceRange());
      llvm::outs() << spec_def << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(spec_decl->getSourceRange(), spec_def);
    }
}
  
void MutatorFrontendAction_341::MutatorASTConsumer_341::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}