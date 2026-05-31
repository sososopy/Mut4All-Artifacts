//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Compound_Literal_With_Array_Initializer_393
 */ 
class MutatorFrontendAction_393 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(393)

private:
    class MutatorASTConsumer_393 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_393(Rewriter &R) : TheRewriter(R) {}
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
        std::unordered_set<string> used_names;
    };
};

//source file
#include "../include/Replace_Compound_Literal_With_Array_Initializer_393.h"

// ========================================================================================================
#define MUT393_OUTPUT 1

void MutatorFrontendAction_393::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CompoundLiteralExpr>("CompoundLiteral")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             CL->getSourceRange());
      llvm::outs() << content << '\n';
      if (content.find('{') == string::npos ||
          content.rfind('}') == string::npos)
        return;
      auto init = content.substr(content.find('{'),
                                 content.rfind('}') - content.find('{') + 1);
      auto type = content.substr(1, content.find(')') - 1);
      llvm::outs() << init << '\n';
      llvm::outs() << type << '\n';
      string arrayname = "mut393";
      int index = 0;
      while (used_names.find(arrayname + to_string(index)) != used_names.end())
        index++;
      arrayname += to_string(index);
      used_names.insert(arrayname);
      auto replacement = type + " " + arrayname + "[] = " + init;
      if (CL->getType()->isPointerType()) {
        replacement += ";\n";
        replacement += arrayname;
      }
      llvm::outs() << replacement << '\n';
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(CL->getSourceRange()), replacement);
    }
}
  
void MutatorFrontendAction_393::MutatorASTConsumer_393::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = compoundLiteralExpr().bind("CompoundLiteral");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}