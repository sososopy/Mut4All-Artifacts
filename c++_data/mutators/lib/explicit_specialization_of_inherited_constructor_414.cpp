//source file
#include "../include/explicit_specialization_of_inherited_constructor_414.h"

// ========================================================================================================
#define MUT414_OUTPUT 1

void MutatorFrontendAction_414::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DerivedDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        //Filter nodes in header files
        if (!DerivedDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                DerivedDecl->getLocation()))
            return;

        // Ensure Derived class has a using declaration for Base constructor
        for (const auto &BaseSpecifier : DerivedDecl->bases()) {
            if (const auto *BaseType = BaseSpecifier.getType()->getAs<clang::RecordType>()) {
                if (const auto *BaseDecl = llvm::dyn_cast<clang::CXXRecordDecl>(BaseType->getDecl())) {
                    for (const auto &Ctor : BaseDecl->ctors()) {
                        if (Ctor->isUserProvided()) {
                            std::string specialization = "\n/*mut414*/template<> " + DerivedDecl->getNameAsString() + 
                                "(" + Ctor->getParamDecl(0)->getType().getAsString() + " x) : " + 
                                BaseDecl->getNameAsString() + "(x) {}\n";
                            Rewrite.InsertTextAfterToken(DerivedDecl->getEndLoc(), specialization);
                            break;
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_414::MutatorASTConsumer_414::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isDerivedFrom(cxxRecordDecl())).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}