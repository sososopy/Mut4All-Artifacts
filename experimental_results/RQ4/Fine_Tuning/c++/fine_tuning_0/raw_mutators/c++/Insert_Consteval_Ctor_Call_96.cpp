//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Consteval_Ctor_Call_96
 */ 
class MutatorFrontendAction_96 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(96)

private:
    class MutatorASTConsumer_96 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_96(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Consteval_Ctor_Call_96.h"

// ========================================================================================================
#define MUT96_OUTPUT 1

void MutatorFrontendAction_96::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ST = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
      //Filter nodes in header files
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;
      if (!ST->isStruct())
        return;
      //Get the source code text of target node
      auto ctors = ST->ctors();
      for (auto ctor : ctors) {
        if (ctor->isConsteval()) {
          //Perform mutation on the source code text by applying string replacement
          auto ctor_content = stringutils::rangetoStr(
              *(Result.SourceManager), ctor->getSourceRange());
          auto struct_name = ST->getNameAsString();
          auto extern_func = "extern constexpr int mut96_extern_func();\n";
          if (ctor_content.rfind('{') != string::npos)
            ctor_content.insert(ctor_content.rfind('{') + 1,
                                "/*mut96*/mut96_extern_func();\n");
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(ctor->getSourceRange()),
              ctor_content);
          Rewrite.ReplaceText(ST->getBeginLoc(), 0, extern_func);
          auto constinit_decl = "constinit " + struct_name + " mut96_obj;\n";
          llvm::outs() << constinit_decl << '\n';
          Rewrite.ReplaceText(ST->getBeginLoc(), 0, constinit_decl);
        }
      }
    }
}
  
void MutatorFrontendAction_96::MutatorASTConsumer_96::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}