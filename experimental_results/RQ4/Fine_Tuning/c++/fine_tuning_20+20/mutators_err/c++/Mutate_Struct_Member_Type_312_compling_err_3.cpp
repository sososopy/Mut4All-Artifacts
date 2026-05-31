//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Struct_Member_Type_312
 */ 
class MutatorFrontendAction_312 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(312)

private:
    class MutatorASTConsumer_312 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_312(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        const clang::TypeDecl *target_type = nullptr;
    };
};

//source file
#include "../include/mutate_struct_member_type_312.h"

// ========================================================================================================
#define MUT312_OUTPUT 1

void MutatorFrontendAction_312::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::FieldDecl>("Members")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto member = stringutils::rangetoStr(*(Result.SourceManager),
                                            MT->getSourceRange());
      if (target_type) {
        auto newtype = target_type->getNameAsString();
        llvm::outs() << "Mut312: " << newtype << '\n';
        llvm::outs() << "Mut312: " << member << '\n';
        stringutils::strReplace(member, MT->getType().getAsString(), newtype);
        llvm::outs() << "Mut312: " << member << '\n';
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(MT->getSourceRange()), member);
      }
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if ((!DL->isStruct() && !DL->isClass() && !DL->isUnion()) ||
          DL->isLambda())
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      auto fields = DL->fields();
      if (fields.begin() == fields.end())
        return;
      auto index = getrandom::getRandomIndex(
          std::distance(fields.begin(), fields.end()) - 1);
      auto target_field = *(std::next(fields.begin(), index));
      llvm::outs() << "Mut312: " << target_field->getNameAsString() << '\n';
      auto target_type = target_field->getType();
      if (target_type->isBuiltinType()) {
        auto type_index = getrandom::getRandomIndex(3);
        ASTContext::GetBuiltinTypeError Error;
        if (type_index == 0)
          this->target_type = Result.Context->getLangOpts().CPlusPlus
                                  ? Result.Context->GetBuiltinType(
                                        BuiltinType::Int, Error)
                                  : Result.Context->GetBuiltinType(
                                        BuiltinType::Int, Error);
        else if (type_index == 1)
          this->target_type = Result.Context->getLangOpts().CPlusPlus
                                  ? Result.Context->GetBuiltinType(
                                        BuiltinType::Float, Error)
                                  : Result.Context->GetBuiltinType(
                                        BuiltinType::Float, Error);
        else if (type_index == 2)
          this->target_type = Result.Context->getLangOpts().CPlusPlus
                                  ? Result.Context->GetBuiltinType(
                                        BuiltinType::Double, Error)
                                  : Result.Context->GetBuiltinType(
                                        BuiltinType::Double, Error);
        else
          assert(false && "Mut312: Builtin type index should be 0,1,2!");
      } else if (target_type->isEnumeralType()) {
        this->target_type = target_type->getAsTagDecl();
      } else if (target_type->isStructureType()) {
        this->target_type = target_type->getAsTagDecl();
      } else if (target_type->isUnionType()) {
        this->target_type = target_type->getAsTagDecl();
      } else if (target_type->isClassType()) {
        this->target_type = target_type->getAsTagDecl();
      } else if (target_type->isTypedefType()) {
        this->target_type = target_type->getAsTagDecl();
      } else if (target_type->isTemplateTypeParmType()) {
        this->target_type = target_type->getAsTagDecl();
      } else if (target_type->isRecordType()) {
        this->target_type = target_type->getAsTagDecl();
      } else if (target_type->isFunctionProtoType()) {
        this->target_type = target_type->getAsTagDecl();
      }
    } else if (auto *TD =
                   Result.Nodes.getNodeAs<clang::TypeDecl>("TypeDecls")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      if (target_type)
        return;
      target_type = TD;
    }
  }

void MutatorFrontendAction_312::MutatorASTConsumer_312::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto member_matcher = fieldDecl().bind("Members");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    auto typedef_matcher = typeDecl().bind("TypeDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(member_matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.addMatcher(typedef_matcher, &callback);
    matchFinder.matchAST(Context);
}