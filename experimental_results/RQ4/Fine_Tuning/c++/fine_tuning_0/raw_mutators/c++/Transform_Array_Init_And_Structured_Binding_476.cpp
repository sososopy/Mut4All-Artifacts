//header file
#pragma once
#include "Mutator_base.h"

/**
 * Transform_Array_Init_And_Structured_Binding_476
 */ 
class MutatorFrontendAction_476 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(476)

private:
    class MutatorASTConsumer_476 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_476(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Transform_Array_Init_And_Structured_Binding_476.h"

// ========================================================================================================
#define MUT476_OUTPUT 1

void MutatorFrontendAction_476::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::VarDecl>("MultiDimArray")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto decl = stringutils::rangetoStr(*(Result.SourceManager),
                                          MT->getSourceRange());
      llvm::outs() << decl << '\n';
      //Perform mutation on the source code text by applying string replacement
      if (decl.find('{') != string::npos && decl.find('}') != string::npos) {
        auto sub = decl.substr(decl.find('{'), decl.rfind('}') - decl.find('{') + 1);
        string new_sub = sub;
        for (int i = 0; i < sub.size(); ++i) {
          if (sub[i] == '{' || sub[i] == '}')
            new_sub[i] = ' ';
        }
        decl.replace(decl.find('{'), decl.rfind('}') - decl.find('{') + 1, new_sub);
        llvm::outs() << decl << '\n';
      }
      if (decl.find(';') != string::npos) {
        decl.replace(decl.find(';'), 1, "; auto [a, b] = arr;");
      }
      decl = "constexpr auto mut_476 = [](){" + decl + "; return 0;};";
      llvm::outs() << decl << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), decl);
    }
}
  
void MutatorFrontendAction_476::MutatorASTConsumer_476::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(hasDescendant(arrayType(hasDescendant(arrayType()))))).bind("MultiDimArray");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}