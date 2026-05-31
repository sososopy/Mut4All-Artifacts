//header file
#pragma once
#include "Mutator_base.h"

/**
 * CTAD_in_Alias_with_Function_Call_139
 */ 
class MutatorFrontendAction_139 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(139)

private:
    class MutatorASTConsumer_139 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_139(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/CTAD_in_Alias_with_Function_Call_139.h"

// ========================================================================================================
#define MUT139_OUTPUT 1

void MutatorFrontendAction_139::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("Alias")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto type_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              MT->getSourceRange());
      llvm::outs() << type_str << '\n';
      auto target_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                MT->getTypeSourceInfo()->getTypeLoc());
      llvm::outs() << target_str << '\n';
      auto target_type = MT->getTypeSourceInfo()->getType();
      if (!target_type->isStructureOrClassType())
        return;
      auto target_decl = target_type->getAsCXXRecordDecl();
      if (target_decl == nullptr)
        return;
      if (target_decl->getDescribedClassTemplate() == nullptr)
        return;
      auto ctors = target_decl->ctors();
      bool flag = false;
      for (auto ctor : ctors) {
        if (ctor->isImplicit())
          continue;
        auto params = ctor->parameters();
        for (auto param : params) {
          if (param->hasDefaultArg()) {
            flag = true;
            break;
          }
        }
        if (flag)
          break;
      }
      if (!flag)
        return;
      //Perform mutation on the source code text by applying string replacement
      type_str = type_str + "/*mut139*/";
      type_str += " " + target_str + "{mut139()}";
      llvm::outs() << type_str << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), type_str);
    }
}
  
void MutatorFrontendAction_139::MutatorASTConsumer_139::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasDecl().bind("Alias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}