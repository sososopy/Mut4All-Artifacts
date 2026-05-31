//header file
#pragma once
#include "Mutator_base.h"

/**
 * Target_Structs_284
 */ 
class MutatorFrontendAction_284 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(284)

private:
    class MutatorASTConsumer_284 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_284(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> cur_structs;
    };
};

//source file
#include "../include/Target_Structs_284.h"

// ========================================================================================================
#define MUT284_OUTPUT 1

void MutatorFrontendAction_284::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isStruct() == false)
      return;
    cur_structs.push_back(DL);
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    if (content.rfind('}') == string::npos)
      return;
    llvm::outs() << content << '\n';
    if (cur_structs.size() > 1) {
      auto last_struct = cur_structs[cur_structs.size() - 2];
      auto last_struct_name = last_struct->getNameAsString();
      llvm::outs() << last_struct_name << '\n';
      content.insert(content.rfind('}'), "\n\t" + last_struct_name + " mut284;");
    } else {
      content.insert(content.rfind('}'), "\n\tint mut284;");
    }
    content.insert(content.rfind('}'), "\n\tstruct { int mut284; };");
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
  }
}
  
void MutatorFrontendAction_284::MutatorASTConsumer_284::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}