//header file
#pragma once
#include "Mutator_base.h"

/**
 * Enum_Scope_Mutation_within_Struct_Context_67
 */ 
class MutatorFrontendAction_67 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(67)

private:
    class MutatorASTConsumer_67 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_67(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> cur_classes;
    };
};

//source file
#include "../include/Enum_Scope_Mutation_within_Struct_Context_67.h"

// ========================================================================================================
#define MUT67_OUTPUT 1

void MutatorFrontendAction_67::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    if (content.find("enum") != string::npos) {
      cur_classes.push_back(DL);
      return;
    }
    auto enumdecl = "\n\t/*mut67*/enum ExampleEnum { X = 0, Y = 1, Z = 2 };";
    if (content.rfind('}') != string::npos)
      content.insert(content.rfind('}'), enumdecl);
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    cur_classes.push_back(DL);
  } else if (auto *FL = Result.Nodes.getNodeAs<clang::FunctionDecl>("Main")) {
    if (!FL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FL->getBeginLoc()))
      return;
    if (FL->isMain() == false)
      return;
    if (cur_classes.empty())
      return;
    size_t index = getrandom::getRandomIndex(cur_classes.size() - 1);
    auto target = cur_classes[index];
    auto target_name = target->getNameAsString();
    auto usingdecl = "/*mut67*/using S = " + target_name + ";\n";
    usingdecl += "using enum S::ExampleEnum;\n";
    usingdecl += "int result = (X | Y);\n";
    llvm::outs() << usingdecl;
    Rewrite.ReplaceText(FL->getBeginLoc(), 0, usingdecl);
  }
}
  
void MutatorFrontendAction_67::MutatorASTConsumer_67::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
  auto main_matcher = functionDecl().bind("Main");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.addMatcher(main_matcher, &callback);
  matchFinder.matchAST(Context);
}