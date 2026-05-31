//header file
#pragma once
#include "Mutator_base.h"

/**
 * Use_Undefined_Struct_With_Builtin_Functions_337
 */ 
class MutatorFrontendAction_337 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(337)

private:
    class MutatorASTConsumer_337 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_337(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::RecordDecl *> cur_undefined;
        std::vector<const clang::RecordDecl *> cur_defined;
    };
};

//source file
#include "../include/use_undefined_struct_with_builtin_functions_337.h"

// ========================================================================================================
#define MUT337_OUTPUT 1

void MutatorFrontendAction_337::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::RecordDecl>("Undefined")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        cur_undefined.push_back(DL);
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::RecordDecl>("Defined")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition())
        cur_defined.push_back(DL);
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      auto fields = DL->fields();
      for (auto field : fields) {
        auto type = field->getType();
        if (type->isPointerType()) {
          auto pointee = type->getPointeeType();
          if (pointee->isRecordType()) {
            auto pointee_decl = pointee->getAsRecordDecl();
            if (pointee_decl->isCompleteDefinition() == false) {
              llvm::outs() << DL->getNameAsString() << '\n';
              Rewrite.ReplaceText(DL->getBeginLoc(), 0, "/*mut337*/");
            }
          }
        }
      }
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("Builtin")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->getBuiltinID() != Builtin::BI__builtin_dump_struct)
        return;
      auto args = DL->parameters();
      if (args.size() != 2)
        return;
      auto arg1 = args[0];
      auto arg2 = args[1];
      auto arg1_type = arg1->getType();
      if (!arg1_type->isPointerType())
        return;
      auto arg1_pointee = arg1_type->getPointeeType();
      if (!arg1_pointee->isRecordType())
        return;
      auto arg1_pointee_decl = arg1_pointee->getAsRecordDecl();
      if (arg1_pointee_decl->isCompleteDefinition() == false) {
        Rewrite.ReplaceText(DL->getBeginLoc(), 0, "/*mut337*/");
        return;
      }
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isMain() == false)
        return;
      if (cur_undefined.empty())
        return;
      if (cur_defined.empty())
        return;
      size_t index = getrandom::getRandomIndex(cur_undefined.size() - 1);
      auto target = cur_undefined[index];
      size_t index2 = getrandom::getRandomIndex(cur_defined.size() - 1);
      auto target2 = cur_defined[index2];
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.rfind('}') != string::npos) {
        auto ins =
            "/*mut337*/" + target->getNameAsString() + "* value = nullptr;\n";
        ins += "__builtin_dump_struct(value, printf);\n";
        content.insert(content.rfind('}'), ins);
      }
      llvm::outs() << content << '\n';
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_337::MutatorASTConsumer_337::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = recordDecl().bind("Undefined");
    auto defined_matcher = recordDecl().bind("Defined");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    auto builtin_matcher = functionDecl().bind("Builtin");
    auto main_matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(defined_matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.addMatcher(builtin_matcher, &callback);
    matchFinder.addMatcher(main_matcher, &callback);
    matchFinder.matchAST(Context);
}