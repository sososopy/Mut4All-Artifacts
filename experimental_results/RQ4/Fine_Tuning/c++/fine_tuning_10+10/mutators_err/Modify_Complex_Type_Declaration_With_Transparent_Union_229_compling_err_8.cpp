//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Complex_Type_Declaration_With_Transparent_Union_229
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
#include "../include/modify_complex_type_declaration_with_transparent_union_229.h"

// ========================================================================================================
#define MUT229_OUTPUT 1

void MutatorFrontendAction_229::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TypedefDecl>("TypedefUnion")) {
        //Filter nodes in header files
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       TD->getLocation()))
            return;

        if (const auto *UT = TD->getUnderlyingType()->getAs<clang::RecordType>()) {
            if (const auto *URD = llvm::dyn_cast<clang::CXXRecordDecl>(UT->getDecl())) {
                if (URD->isUnion() && URD->hasAttr<clang::TransparentUnionAttr>()) {
                    // Get the source code text of target node
                    auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                               TD->getSourceRange());

                    // Perform mutation on the source code text by applying string replacement
                    std::string forwardDecl = "union ForwardDeclaredType;\n";
                    std::string newUnionMember = "union ForwardDeclaredType memberVar;";

                    size_t pos = declaration.find("memberVar;");
                    if (pos != std::string::npos) {
                        declaration.replace(pos, std::string("memberVar;").length(), newUnionMember);
                        declaration = forwardDecl + declaration;
                    }

                    // Replace the original AST node with the mutated one
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), declaration);
                }
            }
        }
    }
}

void MutatorFrontendAction_229::MutatorASTConsumer_229::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typedefDecl(hasType(recordType(hasDeclaration(cxxRecordDecl(
        isUnion(), hasAttr<clang::TransparentUnionAttr>()))))).bind("TypedefUnion");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}