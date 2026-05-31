//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Using_Enum_In_Struct_Class_Enum_67
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
        std::vector<const clang::EnumDecl *> cur_enums;
    };
};

//source file
#include "../include/Insert_Using_Enum_In_Struct_Class_Enum_67.h"

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
    auto enumdecl =
        "\n\t/*mut67*/enum class Foo{A,B,C};\n";
    if (content.rfind('}') != string::npos)
      content.insert(content.rfind('}'), enumdecl);
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    cur_classes.push_back(DL);
  } else if (auto *EL = Result.Nodes.getNodeAs<clang::EnumDecl>("Enums")) {
    if (!EL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   EL->getLocation()))
      return;
    if (EL->isCompleteDefinition() == false)
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), EL->getSourceRange());
    auto enumdecl =
        "\n\t/*mut67*/enum class Foo{A,B,C};\n";
    if (content.rfind('}') != string::npos)
      content.insert(content.rfind('}'), enumdecl);
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(EL->getSourceRange()), content);
    cur_enums.push_back(EL);
  } else if (auto *ML =
                 Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Main")) {
    if (!ML || !Result.Context->getSourceManager().isWrittenInMainFile(
                   ML->getBeginLoc()))
      return;
    if (ML->isMain() == false)
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), ML->getSourceRange());
    if (content.find("using enum") != string::npos)
      return;
    if (content.rfind('}') == string::npos)
      return;
    if (content.find("return") == string::npos)
      return;
    auto return_pos = content.find("return");
    if (!cur_classes.empty()) {
      auto target = cur_classes.back();
      llvm::outs() << target->getNameAsString() << '\n';
      auto ins = "using enum " + target->getNameAsString() + "::Foo;\n";
      content.insert(return_pos, ins);
      content.insert(return_pos, "int f = (A|B);\n");
    } else if (!cur_enums.empty()) {
      auto target = cur_enums.back();
      llvm::outs() << target->getNameAsString() << '\n';
      auto ins = "using enum " + target->getNameAsString() + "::Foo;\n";
      content.insert(return_pos, ins);
      content.insert(return_pos, "int f = (A|B);\n");
    }
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(ML->getSourceRange()), content);
  }
}
  
void MutatorFrontendAction_67::MutatorASTConsumer_67::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto enum_matcher = enumDecl().bind("Enums");
    auto main_matcher = cxxMethodDecl().bind("Main");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(enum_matcher, &callback);
    matchFinder.addMatcher(main_matcher, &callback);
    matchFinder.matchAST(Context);
}