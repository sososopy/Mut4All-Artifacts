//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Invalid_Member_Pointer_Access_258
 */ 
class MutatorFrontendAction_258 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(258)

private:
    class MutatorASTConsumer_258 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_258(Rewriter &R) : TheRewriter(R) {}
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
        const clang::CXXRecordDecl *cur_class = nullptr;
    };
};

//source file
#include "../include/Insert_Invalid_Member_Pointer_Access_258.h"

// ========================================================================================================
#define MUT258_OUTPUT 1

void MutatorFrontendAction_258::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      if (CL->field_empty())
        return;
      cur_class = CL;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isTemplateInstantiation())
        return;
      if (FD->isOverloadedOperator())
        return;
      if (FD->isMain())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (!FD->hasBody())
        return;
      if (cur_class == nullptr)
        return;
      if (!FD->isCXXClassMember()) {
        auto params = FD->parameters();
        bool flag = false;
        for (auto param : params) {
          if (param->getType().getTypePtr()->isRecordType()) {
            auto record = param->getType().getTypePtr()->getAsRecordDecl();
            if (record->getNameAsString() == cur_class->getNameAsString()) {
              flag = true;
              break;
            }
          }
        }
        if (flag == false)
          return;
      }
      auto fields = cur_class->fields();
      std::vector<FieldDecl *> public_field;
      for (auto field : fields) {
        if (field->getAccess() == AccessSpecifier::AS_public)
          public_field.push_back(field);
      }
      if (public_field.empty())
        return;
      auto target_field = public_field[getrandom::getRandomIndex(
          public_field.size() - 1)]; // randomly select a public field
      auto field_name = target_field->getNameAsString();
      auto class_name = cur_class->getNameAsString();
      auto member_pointer_declaration = "int " + class_name + "::* p = &" +
                                        class_name + "::" + field_name + ";\n";
      auto invalid_member_pointer_access = "*p;\n";
      auto ins = "/*mut258*/" + member_pointer_declaration +
                 invalid_member_pointer_access;
      llvm::outs() << ins << '\n';
      Rewrite.ReplaceText(FD->getBody()->getBeginLoc(), 0, ins);
    }
}
  
void MutatorFrontendAction_258::MutatorASTConsumer_258::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto func_matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.matchAST(Context);
}