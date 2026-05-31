//header file
#pragma once
#include "Mutator_base.h"

/**
 * Transparent_Union_With_Recursive_Definition_229
 */ 
class MutatorFrontendAction_229 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(229)

private:
    class MutatorASTConsumer_229 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_229(Rewriter &R) : TheRewriter(R) {}
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
        
    };
};

//source file
#include "../include/Transparent_Union_With_Recursive_Definition_229.h"

// ========================================================================================================
#define MUT229_OUTPUT 1

void MutatorFrontendAction_229::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UD = Result.Nodes.getNodeAs<clang::UnionDecl>("Unions")) {
      if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UD->getLocation()))
        return;
      if (!UD->isCompleteDefinition())
        return;
      if (UD->hasAttr<TransparentUnionAttr>())
        return;
      auto name = UD->getNameAsString();
      if (name == "")
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), UD->getSourceRange());
      llvm::outs() << content << '\n';
      if (content.find(name) != string::npos) {
        llvm::outs() << "find recursive union\n";
        return;
      }
      content.insert(content.rfind('}'), " union " + name + " u; ");
      content += " __attribute__ ((__transparent_union__))";
      llvm::outs() << content << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(UD->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_229::MutatorASTConsumer_229::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = unionDecl().bind("Unions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}