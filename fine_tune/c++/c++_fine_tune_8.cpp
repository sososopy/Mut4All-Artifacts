// header file
#pragma once #include "Mutator_base.h"
/** * Switch_Access_Of_Inherited_Field_UsingDeclaration_8 */
class MutatorFrontendAction_8 : public MutatorFrontendAction
{
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(8)
private:
    class MutatorASTConsumer_8 : public MutatorASTConsumer
    {
    public:
        MutatorASTConsumer_8(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;

    private:
        Rewriter &TheRewriter;
    };
    class Callback : public MatchFinder::MatchCallback
    {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);

    private:
        Rewriter &Rewrite;
        std::vector<const clang::CXXRecordDecl *> cur_classes;
    };
};
// source file
#include "../include/Switch_Access_Of_Inherited_Field_UsingDeclaration_8.h"
// ========================================================================================================
#define MUT8_OUTPUT 1
void MutatorFrontendAction_8::Callback::run(const MatchFinder::MatchResult &Result)
{
    string acc_sep;
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes"))
    {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
            return;
        if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
            return;
        cur_classes.push_back(CL);
    }
    else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived"))
    {
        if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(DL->getLocation()))
            return;
        auto decls = DL->decls();
        for (auto decl : decls)
        {
            if (decl->getIdentifierNamespace() == Decl::IdentifierNamespace::IDNS_Using)
            {
                auto usingdecl = (UsingDecl)decl;
            }
        }
        if (DL->isCompleteDefinition() == false)
            return;
        for (std::size_t j = 0; j < cur_classes.size(); ++j)
        {
            if (DL->isDerivedFrom(cur_classes[j]))
            {
                auto fields = cur_classes[j]->fields();
                std::vector<FieldDecl *> public_field;
                std::vector<FieldDecl *> protected_field;
                for (auto field : fields)
                {
                    if (field->getAccess() == AccessSpecifier::AS_protected)
                        protected_field.push_back(field);
                    else if (field->getAccess() == AccessSpecifier::AS_public)
                        public_field.push_back(field);
                }
                if (!protected_field.empty())
                {
                    acc_sep += " public: ";
                    string classname = cur_classes[j]->getNameAsString();
                    for (auto field : protected_field)
                    {
                        string fieldname = field->getNameAsString();
                        acc_sep += "using " + classname + " :: " + fieldname + "; ";
                        llvm::outs() << acc_sep;

                        auto Err = rep.add(Replacement(*(Result.SourceManager), DL->getEndLoc(), 0, acc_sep));
                    }
                    if (!public_field.empty())
                    {
                        acc_sep += "protected: ";
                        string classname = cur_classes[j]->getNameAsString();
                        for (auto field : public_field)
                        {
                            string fieldname = field->getNameAsString();
                            acc_sep += "using " + classname + " :: " + fieldname + "; ";
                        }
                    }
                }
            }
            acc_sep = " /*mut8*/" + acc_sep;
            llvm::outs() << acc_sep;
            Rewrite.ReplaceText(DL->getEndLoc(), 0, acc_sep);
        }
    }
    void MutatorFrontendAction_8::MutatorASTConsumer_8::HandleTranslationUnit(ASTContext & Context)
    {
        MatchFinder matchFinder;
        DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
        auto derived_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
        auto class_matcher = cxxRecordDecl().bind("Classes");
        Callback callback(TheRewriter);
        matchFinder.addMatcher(derived_matcher, &callback);
        matchFinder.addMatcher(class_matcher, &callback);
        matchFinder.matchAST(Context);
    }
}