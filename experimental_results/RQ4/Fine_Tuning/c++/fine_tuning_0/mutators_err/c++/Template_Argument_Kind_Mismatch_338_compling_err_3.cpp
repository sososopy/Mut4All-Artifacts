//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Argument_Kind_Mismatch_338
 */ 
class MutatorFrontendAction_338 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(338)

private:
    class MutatorASTConsumer_338 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_338(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Argument_Kind_Mismatch_338.h"

// ========================================================================================================
#define MUT338_OUTPUT 1

void MutatorFrontendAction_338::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpecialization")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto args = MT->getTemplateArgs().asArray();
      int num_args = args.size();
      if (num_args == 0)
        return;
      int index = getrandom::getRandomIndex(num_args - 1);
      auto arg = args[index];
      std::string replacement;
      if (arg.getKind() == TemplateArgument::ArgKind::Type) {
        replacement = "5";
      } else {
        replacement = "int";
      }
      auto arg_str = stringutils::rangetoStr(*(Result.SourceManager), arg.getSourceRange());
      llvm::outs() << "mut338: " << arg_str << " -> " << replacement << "\n";
      //Perform mutation on the source code text by applying string replacement
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(arg.getSourceRange(), replacement);
    }
}
  
void MutatorFrontendAction_338::MutatorASTConsumer_338::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}