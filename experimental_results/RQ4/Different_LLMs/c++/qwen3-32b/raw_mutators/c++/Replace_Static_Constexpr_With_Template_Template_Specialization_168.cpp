//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Static_Constexpr_With_Template_Template_Specialization_168
 */ 
class MutatorFrontendAction_168 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(168)
private:
    class MutatorASTConsumer_168 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_168(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Static_Constexpr_With_Template_Template_Specialization_168.h"

// ========================================================================================================
#define MUT168_OUTPUT 1

void MutatorFrontendAction_168::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Record = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("record")) {
        if (!Record || !Result.Context->getSourceManager().isWrittenInMainFile(Record->getLocation()))
            return;

        auto SM = Result.Context->getSourceManager();
        auto LangOpts = Result.Context->getLangOpts();
        auto RecordName = Record->getNameAsString();
        auto RecordRange = Record->getSourceRange();
        std::string OriginalCode = stringutils::rangetoStr(*SM, LangOpts, RecordRange);

        std::string NewCode = "template <typename T, template <typename> class Container>\n";
        NewCode += "struct " + RecordName + " {\n";
        NewCode += "    template <typename U, template <typename> class C>\n";
        NewCode += "    static constexpr bool value = false;\n\n";
        NewCode += "    template <template <typename> class C, typename V>\n";
        NewCode += "    static constexpr bool value<C<V>, C> = true;\n";
        NewCode += "};";

        Rewrite.ReplaceText(RecordRange, NewCode);
    }
}
  
void MutatorFrontendAction_168::MutatorASTConsumer_168::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(
        has(fieldDecl(
            isStatic(),
            hasType(constantArrayType())
        ).bind("field")
    )).bind("record");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}