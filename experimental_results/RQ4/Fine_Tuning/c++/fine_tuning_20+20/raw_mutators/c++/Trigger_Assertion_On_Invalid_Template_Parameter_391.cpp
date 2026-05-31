//header file
#pragma once
#include "Mutator_base.h"

/**
 * Trigger_Assertion_On_Invalid_Template_Parameter_391
 */ 
class MutatorFrontendAction_391 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(391)

private:
    class MutatorASTConsumer_391 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_391(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Trigger_Assertion_On_Invalid_Template_Parameter_391.h"

// ========================================================================================================
#define MUT391_OUTPUT 1

void MutatorFrontendAction_391::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("Template")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto name = MT->getNameAsString();
      llvm::outs() << name << '\n';
      auto args = MT->getTemplateArgs().asArray();
      if (args.size() != 1)
        return;
      auto arg = args[0];
      if (arg.getKind() != TemplateArgument::ArgKind::Type)
        return;
      auto argtype = arg.getAsType();
      if (argtype->isPointerType() == false)
        return;
      auto argstr = stringutils::rangetoStr(*(Result.SourceManager),
                                            MT->getSourceRange());
      auto newstr = argstr;
      llvm::outs() << argstr << '\n';
      newstr = std::regex_replace(newstr, std::regex("const"), "volatile");
      llvm::outs() << newstr << '\n';
      //Perform mutation on the source code text by applying string replacement
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), newstr);
    }
}
  
void MutatorFrontendAction_391::MutatorASTConsumer_391::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("Template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}