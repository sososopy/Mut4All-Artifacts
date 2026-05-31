//header file
#pragma once
#include "Mutator_base.h"

/**
 * Variadic_Template_Arg_Duplication_257
 */ 
class MutatorFrontendAction_257 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(257)

private:
    class MutatorASTConsumer_257 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_257(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Variadic_Template_Arg_Duplication_257.h"

// ========================================================================================================
#define MUT257_OUTPUT 1

void MutatorFrontendAction_257::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("VariadicTemplate")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto name = MT->getNameAsString();
      llvm::outs() << name << '\n';
      auto args = MT->getTemplateArgs().asArray();
      std::vector<string> arg_strs;
      for (auto arg : args) {
        auto arg_str =
            stringutils::rangetoStr(*(Result.SourceManager), arg.getSourceRange());
        arg_strs.push_back(arg_str);
      }
      llvm::outs() << arg_strs.size() << '\n';
      //Perform mutation on the source code text by applying string replacement
      if (arg_strs.size() <= 1)
        return;
      auto last_arg = arg_strs.back();
      arg_strs.push_back(last_arg);
      //Replace the original AST node with the mutated one
      auto mutated_name = name + "<";
      for (auto arg : arg_strs) {
        mutated_name += arg;
        mutated_name += ",";
      }
      mutated_name.pop_back();
      mutated_name += ">";
      llvm::outs() << mutated_name << '\n';
      Rewrite.ReplaceText(MT->getSourceRange(), mutated_name);
    }
}
  
void MutatorFrontendAction_257::MutatorASTConsumer_257::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl(hasAncestor(translationUnitDecl()), has(templateArgumentCountIs(2))).bind("VariadicTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}